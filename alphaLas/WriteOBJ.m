% Write simple object files from coordinates and faces indices
% Copyright ?2017 Ecole polytechnique f?d?rale de Lausanne - EPFL
% Laboratory of Interdisciplinary Performance in Design - LIPID
% @license GNU GPLv3 https://www.gnu.org/licenses/gpl-3.0.en.html


% Author: Giuseppe Peronato, <giuseppe.peronato@epfl.ch>
% Latest modified on 18.07.2017


function WriteOBJ(pts,tri,fname,material)
fid = fopen(fname, 'wt');
if size(material) > 0    
    fprintf(fid, 'mtllib ./material.mtl\n');
end
for i = 1:size(pts,1)
   fprintf(fid, 'v %12.4f %12.4f %4.4f\n', pts(i,:));
end
fprintf(fid, 'o Object\n');
if size(material) > 0    
    fprintf(fid, 'usemtl %s\n',material);
end
for i = 1:size(tri,1)
   fprintf(fid, 'f %g %g %g\n', tri(i,:));
end
fclose(fid);